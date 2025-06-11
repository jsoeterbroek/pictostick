# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'PictoStick'
copyright = '2025, Joost Soeterbroek'
author = 'Joost Soeterbroek'
release = '1.2 PROTOTYPE'

# -- Locales ----------------------
locale_dirs = ['locale/']   # path is example but recommended.
gettext_compact = True     # optional.

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
