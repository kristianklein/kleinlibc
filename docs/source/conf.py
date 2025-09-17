# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os

project = 'kleinlibc'
copyright = '2025, Kristian Klein-Wengel'
author = 'Kristian Klein-Wengel'
release = '0.0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx_rtd_theme',
    'sphinx.ext.todo',
    'breathe'
]

templates_path = ['_templates']
exclude_patterns = []
breathe_projects = {"kleinlibc": os.environ.get("DOXYGEN_XML_DIR", "doxygen/xml/")}
breathe_default_project = "kleinlibc"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
