################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
subm_vcu_ep/SourceCode/source_app/pump/pump.obj: ../subm_vcu_ep/SourceCode/source_app/pump/pump.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/subm_ev_metrics/include" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/include_app/motion" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/subm_motion/include" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/include_app/monitoring" --include_path="D:/git repositories/p.kmc_vcu_ep_unit_test/subm_vcu_ep/SourceCode/subm_tms/include" --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=CPU1 --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="subm_vcu_ep/SourceCode/source_app/pump/pump.pp" --obj_directory="subm_vcu_ep/SourceCode/source_app/pump" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


