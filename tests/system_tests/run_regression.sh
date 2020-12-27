#!/usr/bin/env bash
function failwith() {
  echo "!! $1"
  exit 1
}

which cjson_tester || failwith "cjson_tester not found, make sure it is in your PATH"

JSONTestSuite_dir="JSONTestSuite"

if [[ ! -d ${JSONTestSuite_dir} ]]
then
  echo "-- downloading JSONTestSuite" && \
    wget https://github.com/nst/JSONTestSuite/archive/master.zip && \
    unzip master.zip && \
    mv JSONTestSuite* JSONTestSuite && \
    rm -f master.zip
fi

tests_dir=${JSONTestSuite_dir}/test_parsing
for case_path in ${tests_dir}/*.json
do
  case_name=$(basename "${case_path}")
  [[ "${case_name}" =~ ^i_.* ]] && expected="EXPECT_ANY"
  [[ "${case_name}" =~ ^y_.* ]] && expected="EXPECT_PASS"
  [[ "${case_name}" =~ ^n_.* ]] && expected="EXPECT_FAIL"

  echo "=============================================================================================================="
  echo "test case: ${case_name} (${expected})"
  echo "file path: ${case_path}"
  cat "${case_path}"
  echo
  echo "--------------------------------------------------------------------------------------------------------------"
  output=$(cjson_tester "${case_path}" 2>&1)
  rc=$?
  echo "parser return code: ${rc}"
  echo "${output}"
done
