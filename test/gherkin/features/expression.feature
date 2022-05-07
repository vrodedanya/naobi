Feature: Arithmetic and logical expressions
  Scenario: Adding two positive numbers
    Given script:
      """
      import standard;

      workflow main
      {
        println(5+5);
      }
      """
    Then got 10
  Scenario Outline: Adding two positive numbers multiple cases
    Given script:
      """
      import standard;
      workflow main
      {
        println(input() + input());
      }
      """
    When pass <number1>
    When pass <number2>
    Then got <result>
  Examples:
    | number1 | number2 | result |
    | 5       | 10      | 15     |
    | 156     | 244     | 400    |
    | 100000  | 1000000 | 1100000|

  Scenario: Adding two different types
    Given script:
      """
      import standard;
      workflow main
      {
        println(5 + 5.0);
      }
      """
    Then ends with the code 1

  Scenario: Subtracting two numbers
    Given script:
      """
      import standard;
      workflow main
      {
        println(10-5);
      }
      """
    Then got 5
