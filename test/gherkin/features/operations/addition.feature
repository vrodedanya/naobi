Feature: Add operation
# Integers
  Scenario: Adding two integer numbers
    Given script:
    """
    import standard;
    workflow main
    {
      println(6+6);
    }
    """
    Then got integer 12
    Then ends with the code 0
  Scenario: Add negative number
    Given script:
    """
    import standard;
    workflow main
    {
      println(6 + -6);
    }
    """
    Then got integer 0
    Then ends with the code 0
  Scenario: Add to negative number
    Given script:
    """
    import standard;
    workflow main
    {
      println(-6 + 12);
    }
    """
    Then got integer 6
    Then ends with the code 0
  Scenario: Adding two negative numbers
    Given script:
    """
    import standard;
    workflow main
    {
      println(-6 + -6);
    }
    """
    Then got integer -12
    Then ends with the code 0
  Scenario Outline: Multiple adding cases of integers
    Given script:
    """
    import standard;
    workflow main
    {
      println(int(input()) + int(input()));
    }
    """
    When pass integer <number1>
    When pass integer <number2>
    Then got integer <result>
    Then ends with the code 0
  Examples:
    |number1|number2|result|
    | 10    | 20    | 30   |
    | 30    | 100   | 130  |
    | 99    | 1     | 100  |
    | 11111 | 11111 | 22222|
    | -10   | 10    | 0    |
# Booleans
  Scenario: Adding two booleans
    Given script:
    """
    import standard;
    workflow main
    {
      println(true + false);
    }
    """
    Then got boolean true
    Then ends with the code 0
  Scenario Outline: Adding multiple cases of boolean
    Given script:
    """
    import standard;
    workflow main
    {
      println(bool(input()) + bool(input()));
    }
    """
    When pass boolean <first>
    When pass boolean <second>
    Then got boolean <result>
    Then ends with the code 0
  Examples:
    | first | second | result |
    | true  | true   | true   |
    | true  | false  | true   |
    | false | true   | true   |
    | false | false  | false  |

# Floats
  Scenario: Adding two floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(0.56 + 0.44);
    }
    """
    Then got float 1.0
    Then ends with the code 0
  Scenario Outline: Adding multiple cases of floats
    Given script:
    """
    import standard;
    workflow main
    {
      println(float(input()) + float(input()));
    }
    """
    When pass float <first>
    When pass float <second>
    Then got float <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | 0.5  | 0.5   | 1.0   |
      | 1.24  | 1.76  | 3.0   |
      | 1.24  | 1.5  | 2.74   |
      | -1.24  | 1.5  | 0.26  |

# Strings
  Scenario: Adding two strings
    Given script:
    """
    import standard;
    workflow main
    {
      println("Hello " + "world!");
    }
    """
    Then got string "Hello world!"
    Then ends with the code 0
  Scenario Outline: Adding multiple cases of strings
    Given script:
    """
    import standard;
    workflow main
    {
      println(input() + input());
    }
    """
    When pass string <first>
    When pass string <second>
    Then got string <result>
    Then ends with the code 0
    Examples:
      | first | second | result |
      | "a" | "b"   | "ab"   |
      | "test_"  | "checker"  | "test_checker"   |