
MANMAN=doc/latrace.1
MANTXT=doc/latrace.txt
MANXML=doc/latrace.xml
MANHTML=doc/latrace.html

.PHONY: doc man htmlman

doc: $(MANMAN) $(MANHTML)

$(MANMAN): $(MANTXT)
	$(QUIET_ASCIIDOC)$(ASCIIDOC) -f doc/asciidoc.conf -b docbook -d manpage -o $(MANXML) $(MANTXT)
	$(QUIET_XMLTO)$(XMLTO) -o doc man $(MANXML)

$(MANHTML): $(MANTXT)
	$(QUIET_ASCIIDOC)$(ASCIIDOC) -b xhtml11 -d manpage --unsafe -o $(MANHTML) $(MANTXT)

clean::
	$(call remove, $(MANXML) $(MANHTML) $(MANMAN))

ifneq ($(XMLTO),:)
install:: doc
	$(call install,$(MANMAN),$(mandir)/man1,644)
endif
