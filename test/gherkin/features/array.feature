Feature: Dynamic array
  Scenario: Create array
    Given script:
    """
    workflow main
    {
      integer[] buffer;
    }
    """