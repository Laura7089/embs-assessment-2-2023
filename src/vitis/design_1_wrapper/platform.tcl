# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/design_1_wrapper/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/design_1_wrapper/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {design_1_wrapper}\
-hw {/home/userfs/e/erdd501/source/embs/embs-assessment-2-2203/src/viv_project/design_1_wrapper.xsa}\
-fsbl-target {psu_cortexa53_0} -out {/home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {design_1_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
