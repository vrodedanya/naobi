Feature: Array
  Scenario: Create array of integers
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers;
    }
    """
    Then ends with the code 0
  Scenario: Get length of array
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers;
      println(len(integers));
    }
    """
    Then got integer 0
    Then ends with the code 0
  Scenario: Add element
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers;
      append(integers, 5);
    }
    """
    Then ends with the code 0
  Scenario: Init array
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers = {5};
      println(len(integers));
    }
    """
    Then got integer 1
    Then ends with the code 0
  Scenario: Get element
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers = {5};
      println(get(integers, 0));
    }
    """
    Then got integer 5
    Then ends with the code 0