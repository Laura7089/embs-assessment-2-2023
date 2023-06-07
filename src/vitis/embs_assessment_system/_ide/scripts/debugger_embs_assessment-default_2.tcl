# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/embs_assessment_system/_ide/scripts/debugger_embs_assessment-default_2.tcl
# 
# 
# Usage with xsct:
# In an external shell use the below command and launch symbol server.
# symbol_server -S -s tcp::1534
# To debug using xsct, launch xsct and run below command
# source /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/embs_assessment_system/_ide/scripts/debugger_embs_assessment-default_2.tcl
# 
connect -path [list tcp::1534 tcp:localhost:12345]
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351A780CAA" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351A780CAA-13722093-0"}
fpga -file /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/embs_assessment/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/design_1_wrapper/export/design_1_wrapper/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/embs_assessment/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow /home/userfs/e/erdd501/w2k/source/embs/embs-assessment-2-2203/src/vitis/embs_assessment/Debug/embs_assessment.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
