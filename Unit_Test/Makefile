.PHONY: test clean

ifeq ($(OS),Windows_NT)
test:
	powershell -NoProfile -ExecutionPolicy Bypass -File build_and_run.ps1
clean:
	powershell -NoProfile -Command "if (Test-Path build) { Remove-Item -Recurse -Force build }"
else
test:
	bash build_and_run.sh
clean:
	rm -rf build
endif
