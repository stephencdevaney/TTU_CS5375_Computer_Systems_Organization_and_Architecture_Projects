.PHONY: clean All

All:
	@echo "----------Building project:[ Project1_Cache_Simulator - Debug ]----------"
	@cd "Project1_Cache_Simulator" && "$(MAKE)" -f  "Project1_Cache_Simulator.mk"
clean:
	@echo "----------Cleaning project:[ Project1_Cache_Simulator - Debug ]----------"
	@cd "Project1_Cache_Simulator" && "$(MAKE)" -f  "Project1_Cache_Simulator.mk" clean
