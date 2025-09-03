#!/usr/bin/env bash
# exec_tests.sh — Minishell Execution Test Harness (no UI download needed)
# Usage:
#   chmod +x exec_tests.sh
#   ./exec_tests.sh
# Optional:
#   MINISHELL=./minishell ./exec_tests.sh

set -euo pipefail

# --- Config ------------------------------------------------------------------
: "${MINISHELL:=./minishell}"   # path ke executable minishell
: "${TIMEOUT_SEC:=8}"           # timeout opsional per test (dipakai jika 'timeout' tersedia)
TMPROOT="$(mktemp -d /tmp/minish_exec.XXXXXX)"
trap 'rm -rf "$TMPROOT"' EXIT

RED=$'\033[31m'; GREEN=$'\033[32m'; YELLOW=$'\033[33m'; BOLD=$'\033[1m'; RESET=$'\033[0m'

have_timeout=0
command -v timeout >/dev/null 2>&1 && have_timeout=1

# --- Helpers -----------------------------------------------------------------
run_shell_with_input() {
  # $1 = label, $2 = shell_cmd ("bash" atau "$MINISHELL"), stdin via heredoc
  local label="$1" shell_bin="$2"
  local in="$TMPROOT/in.$RANDOM" out="$TMPROOT/out.$RANDOM" err="$TMPROOT/err.$RANDOM" st="$TMPROOT/st.$RANDOM"

  # cat stdin ke file, pastikan shell keluar: append 'exit' kalau belum ada
  cat > "$in"
  if ! tail -n 1 "$in" | grep -qE '^[[:space:]]*exit([[:space:]]+[0-9]+)?[[:space:]]*$'; then
    echo "exit" >> "$in"
  fi

  if [ "$shell_bin" = "bash" ]; then
    if [ "$have_timeout" -eq 1 ]; then
      timeout "$TIMEOUT_SEC" bash --noprofile --norc <"$in" >"$out" 2>"$err" || true
    else
      bash --noprofile --norc <"$in" >"$out" 2>"$err" || true
    fi
    printf "%d\n" "$?" >"$st"
  else
    if [ "$have_timeout" -eq 1 ]; then
      timeout "$TIMEOUT_SEC" bash -c "$shell_bin <'$in' >'$out' 2>'$err'"; echo "$?" >"$st"
    else
      bash -c "$shell_bin <'$in' >'$out' 2>'$err'"; echo "$?" >"$st"
    fi
  fi
  echo "$in|$out|$err|$st"
}

compare_minishell_vs_bash() {
  # $1=label, $2=minishell_tuple, $3=bash_tuple
  local label="$1" mini="$2" bashr="$3"
  IFS='|' read -r mi_in mi_out mi_err mi_st <<<"$mini"
  IFS='|' read -r bs_in bs_out bs_err bs_st <<<"$bashr"

  local st_mini st_bash
  st_mini=$(cat "$mi_st"); st_bash=$(cat "$bs_st")

  local failed=0
  if ! diff -u "$bs_out" "$mi_out" >"$TMPROOT/diff.$RANDOM"; then
    echo "${RED}[FAIL]${RESET} $label — STDOUT differ"
    sed -n '1,120p' "$TMPROOT"/diff.*
    failed=1
  else
    echo "${GREEN}[PASS]${RESET} $label — STDOUT"
  fi

  if [ "$st_mini" != "$st_bash" ]; then
    echo "${RED}[FAIL]${RESET} $label — exit status minishell=$st_mini bash=$st_bash"
    failed=1
  else
    echo "${GREEN}[PASS]${RESET} $label — exit status $st_mini"
  fi

  if ! diff -u "$bs_err" "$mi_err" >"$TMPROOT/differr.$RANDOM"; then
    echo "${YELLOW}[WARN]${RESET} $label — STDERR differ (expected: prefix beda)"
  fi

  return "$failed"
}

section() { echo -e "\n${BOLD}=== $* ===${RESET}"; }

# --- Test Suites -------------------------------------------------------------

test_simple_commands() {
  section "Simple commands (absolute path, args, empty/spaces)"
  local INPUT
  read -r -d '' INPUT <<'EOS'
/bin/echo hello
/bin/ls >/dev/null
/bin/true
/bin/false
/bin/echo alpha beta gamma
/bin/printf "%s-%s\n" one two

# empty lines & spaces

/bin/echo done
EOS
  local mini bashr
  mini=$(run_shell_with_input simple "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input simple bash <<<"$INPUT")
  compare_minishell_vs_bash "simple commands" "$mini" "$bashr"
}

test_builtins_core() {
  section "Builtins (echo/pwd/export/env/unset/exit)"
  local INPUT
  read -r -d '' INPUT <<'EOS'
echo hello
echo -n a b c
pwd
export FOO=bar
env | grep '^FOO='
unset FOO
env | grep '^FOO=' || true
exit 0
EOS
  local mini bashr
  mini=$(run_shell_with_input builtins "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input builtins bash <<<"$INPUT")
  compare_minishell_vs_bash "builtins core" "$mini" "$bashr"
}

test_path_resolution() {
  section "PATH resolution (unset, set, order)"
  local INPUT
  read -r -d '' INPUT <<'EOS'
/bin/echo PATH-START
unset PATH
ls || echo "ls failed as expected"
export PATH=/bin:/usr/bin
ls >/dev/null
printf "OK\n"
EOS
  local mini bashr
  mini=$(run_shell_with_input pathset "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input pathset bash <<<"$INPUT")
  compare_minishell_vs_bash "PATH unset/set & search order" "$mini" "$bashr"
}

test_redirections_and_heredoc() {
  section "Redirections < > >> and heredoc <<"
  local F="$TMPROOT/rin.txt" OUT="$TMPROOT/rout.txt"
  echo "line1" >"$F"; echo "line2" >>"$F"
  local INPUT
  read -r -d '' INPUT <<EOS
export TF="$F"
export TO="$OUT"
cat < "\$TF"
echo out1 > "\$TO"
echo out2 >> "\$TO"
cat "\$TO"
cat <<EOF
aaa
bbb
EOF
EOS
  local mini bashr
  mini=$(run_shell_with_input redirs "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input redirs bash <<<"$INPUT")
  compare_minishell_vs_bash "redirections & heredoc" "$mini" "$bashr"
}

test_pipes() {
  section "Pipes (success & error)"
  local INPUT
  read -r -d '' INPUT <<'EOS'
printf "a\nb\nc\n" | grep b | wc -l
ls /does_not_exist | wc -l
echo hello | cat | cat | wc -c
EOS
  local mini bashr
  mini=$(run_shell_with_input pipes "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input pipes bash <<<"$INPUT")
  compare_minishell_vs_bash "pipes chain" "$mini" "$bashr"
}

test_wildcard_bonus() {
  section "Wildcard * (current directory, skip hidden kecuali pattern diawali .)"
  local D="$TMPROOT/wild"
  mkdir -p "$D"; touch "$D/a.txt" "$D/b.txt" "$D/.hidden" "$D/a.c"
  local INPUT
  read -r -d '' INPUT <<EOS
cd "$D"
printf "%s\n" *
printf "%s\n" *.txt
printf "%s\n" .*
EOS
  local mini bashr
  mini=$(run_shell_with_input wildcard "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input wildcard bash <<<"$INPUT")
  compare_minishell_vs_bash "wildcard patterns" "$mini" "$bashr"
}

test_and_or_paren_bonus() {
  section "AND/OR & parenthesis semantics"
  local INPUT
  read -r -d '' INPUT <<'EOS'
false && echo should_not_print
true && echo should_print
false || echo alt_print
(true && echo X) || echo Y
(false && echo A) || (echo B && echo C)
EOS
  local mini bashr
  mini=$(run_shell_with_input andor "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input andor bash <<<"$INPUT")
  compare_minishell_vs_bash "AND/OR/()" "$mini" "$bashr"
}

test_not_found_and_perm() {
  section "Command-not-found (127) & Permission denied (126)"
  local NOEXEC="$TMPROOT/noexec.sh"
  echo 'echo hi' > "$NOEXEC"; chmod 644 "$NOEXEC"   # not executable
  local INPUT
  read -r -d '' INPUT <<EOS
idontexistcommand
"$NOEXEC"
EOS
  local mini bashr
  mini=$(run_shell_with_input errors "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input errors bash <<<"$INPUT")
  compare_minishell_vs_bash "errors 127/126" "$mini" "$bashr"
}

test_export_in_pipeline_no_parent_effect() {
  section "Export di pipeline tidak mempengaruhi parent env"
  local INPUT
  read -r -d '' INPUT <<'EOS'
env | grep '^PIPE_VAR=' || echo "not-set"
export PIPE_VAR=ok | cat
env | grep '^PIPE_VAR=' || echo "not-set"
EOS
  local mini bashr
  mini=$(run_shell_with_input pipe_export "$MINISHELL" <<<"$INPUT")
  bashr=$(run_shell_with_input pipe_export bash <<<"$INPUT")
  compare_minishell_vs_bash "export in pipeline isolation" "$mini" "$bashr"
}

# --- Main --------------------------------------------------------------------
echo "${BOLD}Minishell execution test harness${RESET}"
echo "MINISHELL = $MINISHELL"
[ -x "$MINISHELL" ] || { echo "${RED}Executable not found or not executable${RESET}"; exit 1; }

fail=0
test_simple_commands        || fail=1
test_builtins_core          || fail=1
test_path_resolution        || fail=1
test_redirections_and_heredoc || fail=1
test_pipes                  || fail=1
test_not_found_and_perm     || fail=1

# Bonus (aktifkan kalau fitur ada)
test_wildcard_bonus         || fail=1
test_and_or_paren_bonus     || fail=1
test_export_in_pipeline_no_parent_effect || fail=1

echo -e "\n${BOLD}Summary:${RESET} $([ "$fail" -eq 0 ] && echo "${GREEN}ALL PASS${RESET}" || echo "${RED}SOME FAILURES${RESET}")"
exit "$fail"
