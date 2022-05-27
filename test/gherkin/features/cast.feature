Feature: Functions for casting types to each other
  Scenario: Cast string to integer
    Given script:
    """
    import standard;
    workflow main
    {
      println(int("5") + 5);
    }
    """
    Then got integer 10
    Then ends with the code 0
  Scenario: Cast float to integer
    Given script:
    """
    import standard;
    workflow main
    {
      println(int(5.5) + 5);
    }
    """
    Then got integer 10
    Then ends with the code 0
  Scenario: Cast boolean to integer
    Given script:
    """
    import standard;
    workflow main
    {
      println(int(true) + 5);
    }
    """
    Then got integer 6
    Then ends with the code 0
  Scenario: Cast integer to string
    Given script:
    """
    import standard;
    workflow main
    {
      println("Integer " + str(5));
    }
    """
    Then got string "Integer 5"
    Then ends with the code 0
  Scenario: Cast float to string
    Given script:
    """
    import standard;
    workflow main
    {
      println("Float " + str(5.5));
    }
    """
    Then got string "Float 5.5"
    Then ends with the code 0
  Scenario: Cast boolean to string
    Given script:
    """
    import standard;
    workflow main
    {
      println("Boolean " + str(true));
    }
    """
    Then got string "Boolean true"
    Then ends with the code 0
  Scenario: Cast integer to float
    Given script:
    """
    import standard;
    workflow main
    {
      println(5.5 + float(5));
    }
    """
    Then got float 10.5
    Then ends with the code 0
  Scenario: Cast string to float
    Given script:
    """
    import standard;
    workflow main
    {
      println(5.5 + float("4.5"));
    }
    """
    Then got float 10.0
    Then ends with the code 0
  Scenario: Cast boolean to float error
    Given script:
    """
    import standard;
    workflow main
    {
      println(5.5 + float(true));
    }
    """
    Then fails with compilation error and code 53
  Scenario: Cast integer to boolean
    Given script:
    """
    import standard;
    workflow main
    {
      println(bool(1));
    }
    """
    Then got boolean true
    Then ends with the code 0
  Scenario: Cast string to boolean
    Given script:
    """
    import standard;
    workflow main
    {
      println(bool("true"));
    }
    """
    Then got boolean true
    Then ends with the code 0