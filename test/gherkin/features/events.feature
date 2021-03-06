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
  Scenario: Event with variable that already exist in workflow
    Given script:
    """
    import standard;
    event JustEvent{string str}
    workflow main
    {
      string str = "I already exist";
      arise JustEvent(str: "Hello");
      string second = str;
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.str);
    }
    """
    Then got string "Hello"
    Then ends with the code 0
  Scenario: Arising event in function
    Given script:
    """
    import standard;
    event JustEvent{string str}
    function func()
    {
      arise JustEvent(str: "Hello");
    }
    workflow main
    {
      func();
      integer test = 5 + 5;
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.str);
    }
    """
    Then got string "Hello"
    Then ends with the code 0
  Scenario: Arising event and pass expression
    Given script:
    """
    import standard;
    event JustEvent{string str}
    workflow main
    {
      arise JustEvent(str: "Hello" + " world!");
    }
    workflow test
    target JustEvent
    {
      println(JustEvent.str);
    }
    """
    Then got string "Hello world!"
    Then ends with the code 0