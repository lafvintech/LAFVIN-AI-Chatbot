# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

#import sphinx_conestack_theme
from pygments.lexers import Python3Lexer
import time

project = 'AIChatBot'
copyright = f'{time.localtime().tm_year}, LAFVIN'
author = 'LAFVIN'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "myst_parser",
    "sphinx_copybutton",
    "sphinxcontrib.video"
]

pygments_lexers = {
    "python-repl": Python3Lexer(),
}

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

templates_path = ['_templates']

exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'

# html_static_path = ['_static']
# html_logo = '_static/Logo2.png'

html_theme_options = {
     'logo_only': True,
 }