Feature: Events
  Scenario: Create event
    Given script:
    """
    event JustEvent
    {
      integer check;
      float second;
    }
    """
    Then ends with the code 0
  Scenario: Arise event
    Given script:
    """
    event JustEvent
    {
      integer first;
      float second;
    }
    workflow main
    {
      arise JustEvent(5, 5.0);
    }
    """
    Then ends with the code 0