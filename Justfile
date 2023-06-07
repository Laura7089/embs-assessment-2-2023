#!/bin/env -S just --justfile

REPORT_DIR := "./report"
PROJ_DIR := "./src"
SUB_DIR := "./submission"

SUB_FILE := "./embs2_submission.zip"
SUB_PRINT := "Submission Ready!
    path: " + SUB_FILE + "
    hash: $(md5sum \"" + SUB_FILE + "\" | cut -d \" \" -f1)
    size: $(du -h \"" + SUB_FILE + "\" | cut -f1)
    num_files: $(find \"" + SUB_DIR + "\" -type f | wc -l)
"

RESOURCE_LINK_FILE := "./2022_2023_resources.lnk"
BROWSER := env_var_or_default("BROWSER", "firefox")

# compile submission zip
submission: clean report && stats
    mkdir -pv "{{ SUB_DIR }}"
    cp -v "{{ REPORT_DIR }}/submission.pdf" "{{ SUB_DIR }}/report.pdf"
    cp -rv "{{ PROJ_DIR }}/viv_project" "{{ SUB_DIR }}"
    cp -rv "{{ PROJ_DIR }}/vitis" "{{ SUB_DIR }}"
    ouch compress "{{ SUB_DIR }}" "{{ SUB_FILE }}"

# print stats for the submission file
stats:
    #!/bin/bash
    set -euo pipefail
    if [ ! -f "{{ SUB_FILE }}" ]; then
        echo "No submission file found, try 'just submission'"
        exit 1
    else
        echo "{{ SUB_PRINT }}"
    fi

# open the EMBS wiki
wiki browser=BROWSER:
    "{{ browser }}" $(cat "{{ RESOURCE_LINK_FILE }}")

# open the assessment rubric
rubric viewer="okular":
    "{{ viewer }}" rubric.pdf &

[private]
report:
    just "{{ REPORT_DIR }}/" report

# clean submission and intermediate files
clean:
    @# NOTE: don't clean the report directory since we only grab the pdf
    just "{{ PROJ_DIR }}/" clean
    rm -rfv "{{ SUB_DIR }}" "{{ SUB_FILE }}"
