Feature: If else statement

  Scenario: Single if
    Given script:
    """
    import standard;
    workflow main
    {
      if (5 == 5)
      {
        println("TRUE");
      }
    }
    """
    Then got string "TRUE"
    Then ends with the code 0

  Scenario: If with else
    Given script:
    """
    import standard;
    workflow main
    {
      if (5 != 5)
      {
        println("TRUE");
      }
      else
      {
        println("FALSE");
      }
    }
    """
    Then got string "FALSE"
    Then ends with the code 0

  Scenario: if without brackets
    Given script:
    """
    import standard;
    workflow main
    {
      if 5 != 5
      {
        println("TRUE");
      }
      else
      {
        println("FALSE");
      }
    }
    """
    Then got string "FALSE"
    Then ends with the code 0
  Scenario: Not boolean in logical expression
    Given script:
    """
    workflow main
    {
      if 5
      {
      }
    }
    """
    Then fails with compilation error 53