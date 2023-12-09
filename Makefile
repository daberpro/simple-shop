All:
	@echo "----------Building project:[ web-tokoh - Debug ]----------"
	@cd "web-tokoh" && $(MAKE) /nologo /f  "web-tokoh.mk"
clean:
	@echo "----------Cleaning project:[ web-tokoh - Debug ]----------"
	@cd "web-tokoh" && $(MAKE) /nologo /f  "web-tokoh.mk" clean
.PHONY: clean All

