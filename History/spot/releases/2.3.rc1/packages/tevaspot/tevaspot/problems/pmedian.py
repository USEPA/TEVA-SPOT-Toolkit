
__all__ = []

from tevaspot.core import *
from simple import SimplePerfectSensorProblem


class PMedianProblem(SimplePerfectSensorProblem):
    """
    A base class for variants of p-median problems
    """

    def __init__(self):
        """
        Initialize here
        """
        SimplePerfectSensorProblem.__init__(self, "mean")

ProblemRegistration("simple-mean", PMedianProblem, description="Minimize mean with simple perfect sensors")
ProblemRegistration("pmedian", PMedianProblem, description="Minimize mean with simple perfect sensors")
