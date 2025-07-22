# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXPYTHON  = venv/bin/python
SPHINXBUILD   = $(SPHINXPYTHON) -m sphinx
SOURCEDIR     = .
BUILDDIR      = _build

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile latexpdf-en latexpdf-nl

# Custom PDF targets for specific languages
latexpdf-en:
	@echo "Building English PDF..."
	@$(SPHINXBUILD) -M latexpdf "$(SOURCEDIR)" "$(BUILDDIR)/en" $(SPHINXOPTS) -D language=en
	@echo "Build finished. The English PDF can be found in $(BUILDDIR)/en/latex/"

latexpdf-nl:
	@echo "Building Dutch PDF..."
	@$(SPHINXBUILD) -M latexpdf "$(SOURCEDIR)" "$(BUILDDIR)/nl" $(SPHINXOPTS) -D language=nl
	@echo "Build finished. The Dutch PDF can be found in $(BUILDDIR)/nl/latex/"

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
