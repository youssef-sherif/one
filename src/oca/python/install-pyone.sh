#!/bin/bash

pip install setuptools wheel generateds # Build dependencies
make 									# Create python package from xsd
python setup.py sdist bdist_wheel 		# Create python package
sudo pip install dist/*.tar.gz 			# Install python package