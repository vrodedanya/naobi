Feature: Multiplication operation
# Integers
  Scenario: Multiply two integer numbers
    Given script:
    """
    import standard;
    workflow main
    {
      println(6*6);
    }
    """
    Then got integer 36
    Then ends with the code 0
  Scenario: Multiple negative number
    Given script:
    """
    import standard;
    workflow main
    {
      println(6 * -6);
    }
    """
    Then got integer -36
    Then ends with the code 0
  Scenario Outline: Multiple multiply cases of integers
    Given script:
    """
    import standard;
    workflow main
    {
      println(int(input()) * int(input()));
    }
    """
    When pass integer <number1>
    When pass integer <number2>
    Then got integer <result>
    Then ends with the code 0
  Examples:
    |number1|number2|result|
    | 10    | 20    | 200  |
    | 30    | 100   | 3000 |
    | 99    | 1     | 99   |
    | 11    | 12    | 132  |
    | -10   | 10    | -100 |
# Booleans
  Scenario: Multiply two booleans
    Given script:
    """
    import standard;
    workflow main
    {
      println(true * false);
    }
    """
    Then got boolean false
    Then ends with the code 0
  Scenario Outline: Multiply multiple cases of boolean
    Given script:
    """
    import standard;
    workflow main
    {
      println(bool(input()) * bool(input()));
    }
    """
    When pass boolean <first>
    When pass boolean <second>
    Then got boolean <result>
    Then ends with the code 0
  Examples:
    | first | second | result |
    | true  | true   | true   |
    | true  | false  | false   |
    | false | true   | false   |
    | false | false  | false  |

# Floats
  Scenario: Multiply two floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(0.56 * 0.44);
    }
    """
    # Python round?
    Then got float 0.2464
    Then ends with the code 0
  Scenario Outline: Multiply multiple cases of floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(float(input()) * float(input()));
    }
    """
    When pass float <first>
    When pass float <second>
    Then got float <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | 0.5  | 0.5   | 0.25   |
      | 1.0  | 0.1  | 0.1   |
      | 1.42  | 0.1  | 0.142   |
      | -1.5  | 2.0  | -3.0  |

# Strings
  Scenario: Multiply two strings
    Given script:
    """
    import standard;
    workflow main
    {
      println("Hello " * "world!");
    }
    """
    Then fails with compilation error and code 53
  Scenario: Multiply string with integer
    Given script:
    """
    import standard;
    workflow main
    {
      println("hello " * 5);
    }
    """
    Then got string "hello hello hello hello hello "
    Then ends with the code 0