################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
tpms/tpms_unity.obj: ../tpms/tpms_unity.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/include_app/motion" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/subm_motion/include" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/include_app/monitoring" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/subm_tms/include" --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="tpms/tpms_unity.pp" --obj_directory="tpms" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


