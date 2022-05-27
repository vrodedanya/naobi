Feature: Division operation
# Integers
  Scenario: Divide two integer numbers
    Given script:
    """
    import standard;
    workflow main
    {
      println(6 / 3);
    }
    """
    Then got integer 2
    Then ends with the code 0
  Scenario Outline: Multiple divide cases of integers
    Given script:
    """
    import standard;
    workflow main
    {
      println(int(input()) / int(input()));
    }
    """
    When pass integer <number1>
    When pass integer <number2>
    Then got integer <result>
    Then ends with the code 0
  Examples:
    |number1|number2|result|
    | 10    | 20    | 0  |
    | 6    | 2   | 3 |
    | 99    | 1     | 99   |
    | 5    | 2    | 2  |
    | 100   | -10    | -10 |
# Booleans
  Scenario: Divide two booleans
    Given script:
    """
    import standard;
    workflow main
    {
      println(true / false);
    }
    """
    Then fails with compilation error and code 53
# Floats
  Scenario: Divide two floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(5.0 / 2.0);
    }
    """
    Then got float 2.5
    Then ends with the code 0
  Scenario Outline: Divide multiple cases of floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(float(input()) / float(input()));
    }
    """
    When pass float <first>
    When pass float <second>
    Then got float <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | 0.5  | 0.5   | 1.0   |
      | 1.0  | 0.1  | 10.0   |
      | 2.5  | 0.1  | 25.0  |
      | 4.3  | -2.0  | -2.15  |

# Strings
  Scenario: Divide two strings
    Given script:
    """
    import standard;
    workflow main
    {
      println("Hello " / "world!");
    }
    """
    Then fails with compilation error and code 53
