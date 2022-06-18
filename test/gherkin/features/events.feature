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
  Scenario: Arise event with expression
    Given script:
    """
    event JustEvent
    {
      integer first;
      float second;
    }
    workflow main
    {
      integer var = 3;
      arise JustEvent(5 + 10 + var, 5.0);
    }
    """
    Then ends with the code 0
  Scenario: Arise event and print value in triggered workflow
    Given script:
    """
    import standard;
    event JustEvent
    {
      integer first;
      float second;
    }
    workflow main
    {
      integer var = 3;
      arise JustEvent(5 + 10 + var, 5.0);
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.first);
      println(JustEvent.second);
    }
    """
    Then got integer 18
    Then got float 5.0
    Then ends with the code 0

  Scenario: Arise event with string which contains ','
    Given script:
    """
    import standard;
    event JustEvent{string str}

    workflow main
    {
      arise JustEvent("test, str");
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.str);
    }
    """
    Then got string "test, str"
    Then ends with the code 0

  Scenario: Arise event with string which contains ':'
    Given script:
    """
    import standard;
    event JustEvent{string str}

    workflow main
    {
      arise JustEvent("test: str");
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.str);
    }
    """
    Then got string "test: str"
    Then ends with the code 0