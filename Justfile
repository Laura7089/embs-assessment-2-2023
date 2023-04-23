#!/bin/env -S just --justfile

REPORT_DIR := "./report"
PROJ_DIR := "./src"
SUB_DIR := "./submission"

SUB_FILE := "./embs2_submission.zip"
SUB_PRINT := "Created:
    file: " + SUB_FILE + "
    size: $(du -h \"" + SUB_FILE + "\" | cut -f1)
    hash: $(md5sum \"" + SUB_FILE + "\" | cut -d \" \" -f1)"

submission: clean report
    mkdir -pv "{{ SUB_DIR }}"
    cp -v "{{ REPORT_DIR }}/submission.pdf" "{{ SUB_DIR }}/report.pdf"
    cp -rv "{{ PROJ_DIR }}" "{{ SUB_DIR }}"
    ouch compress "{{ SUB_DIR }}" "{{ SUB_FILE }}"
    @echo
    @echo "{{ SUB_PRINT }}"

report:
    just "{{ REPORT_DIR }}/" report

# clean submission and intermediate files
clean:
    @# NOTE: don't clean the report directory since we only grab the pdf
    just "{{ PROJ_DIR }}/" clean
    rm -rfv "{{ SUB_DIR }}" "{{ SUB_FILE }}"
