Feature: Subtract operation
# Integers
  Scenario: Subtract two integer numbers
    Given script:
    """
    import standard;
    workflow main
    {
      println(10-6);
    }
    """
    Then got integer 4
    Then ends with the code 0
  Scenario Outline: Multiple subtracting cases of integers
    Given script:
    """
    import standard;
    workflow main
    {
      println(input() - input());
    }
    """
    When pass integer <number1>
    When pass integer <number2>
    Then got integer <result>
    Then ends with the code 0
  Examples:
    |number1|number2|result|
    | 10    | 20    | -10   |
    | 100    | 30   | 70  |
    | 99    | 1     | 98  |
    | 11111 | 11111 | 0|
    | -10   | 10    | -20    |
# Booleans
  Scenario: Subtracting two booleans
    Given script:
    """
    import standard;
    workflow main
    {
      println(true - false);
    }
    """
    Then fails with compilation error and code 53

# Floats
  Scenario: Subtracting two floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(0.56 - 0.44);
    }
    """
    Then got float 0.12
    Then ends with the code 0
  Scenario Outline: Subtracting multiple cases of floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(input() - input());
    }
    """
    When pass float <first>
    When pass float <second>
    Then got float <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | 0.5   | 0.5    | 0.0    |
      | 1.24  | 1.76   | -0.52  |
      | 2.24  | 1.76   | 0.48    |

# Strings
  Scenario: Subtracting two strings
    Given script:
    """
    import standard;
    workflow main
    {
      println("TestRemoveTest" - "Remove");
    }
    """
    Then got string "TestTest"
    Then ends with the code 0
  Scenario Outline: Subtracting multiple cases of strings
    Given script:
    """
    import standard;
    workflow main
    {
      println(input() - input());
    }
    """
    When pass string <first>
    When pass string <second>
    Then got string <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | "a" | "b"   | "a"   |
      | "Password"  | "a"  | "Pssword"   |
      | "Lalalalala"  | "a"  | "Lllll"   |