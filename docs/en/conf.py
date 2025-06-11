# Configuration file for the Sphinx documentation builder.

import os
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'PictoStick'
copyright = '2025, Joost Soeterbroek'
author = 'Joost Soeterbroek'
release = '1.3 PROTOTYPE'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ["sphinx_rtd_dark_mode"]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

#-html_theme = 'alabaster'

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_logo = "pictostick_logo.png"
html_theme_options = {
    'logo_only': True,
}

html_build_dir = os.environ.get('READTHEDOCS_OUTPUT', 'docs/en/_build/html')
