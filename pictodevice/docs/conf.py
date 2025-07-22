# Configuration file for the Sphinx documentation builder.

import os
import re

# -- Dynamically read version from C++ header --------------------------------
def get_version_from_header():
    try:
        with open('../include/common.h', 'r') as f:
            content = f.read()
            major = re.search(r'#define PD_VERSION_MAJOR\s+(\d+)', content).group(1)
            minor = re.search(r'#define PD_VERSION_MINOR\s+(\d+)', content).group(1)
            patch = re.search(r'#define PD_VERSION_PATCH\s+(\d+)', content).group(1)
            return f"{major}.{minor}.{patch}"
    except Exception as e:
        print(f"Error reading version from header: {e}")
        return "1.0.0" # Fallback version

#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'PictoStick'
copyright = '2025, Joost Soeterbroek'
author = 'Joost Soeterbroek'
release = get_version_from_header()

# -- Locales ----------------------
locale_dirs = ['locale/']
gettext_uuid = True
gettext_compact = "docs"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'venv']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

#-html_theme = 'alabaster'

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_logo = "pictostick_logo.png"
html_theme_options = {
    'logo_only': True,
}

html_build_dir = os.environ.get('READTHEDOCS_OUTPUT', 'docs/_build/html')
