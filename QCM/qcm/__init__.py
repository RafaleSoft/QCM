# Initialise Meteo application
from flask import Flask

import qcm.qcmMain

# Global variables
theServices = qcm.qcmMain.QCM()
application: Flask = theServices.qcm
