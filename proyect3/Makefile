
BEAMER_PRESENTATION_DIR_DIR=presentation

build: proyect $(BEAMER_PRESENTATION_DIR_DIR)

.PHONY: proyect
proyect:
	$(MAKE) -C src

$(BEAMER_PRESENTATION_DIR_DIR):
	mkdir -p $@

clean:
	cd src && make clean
	rm -rf $(BEAMER_PRESENTATION_DIR_DIR)/
