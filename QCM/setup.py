import os
from setuptools import find_packages, setup


# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...
def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


setup(
    name="qcm",
    version="1.1.0",
    author="Fanny & Fabrice Ferrand",
    author_email="fabfer@wanadoo.fr",
    description="Site web de d'édition et de génération de QCM Mathématiques",
    license="GPLv3",
    keywords="QCM mathematics",
    url="https://github.com/RafaleSoft/QCM.git",

    long_description=read('README'),
    packages=find_packages(),
    include_package_data=True,

    zip_safe=False,
    install_requires=['flask', 'paramiko'],

    classifiers=[
        "Development Status :: 3 - Beta",
        "Topic :: Utilities",
        "License :: GPLv3 License",
    ],
)
