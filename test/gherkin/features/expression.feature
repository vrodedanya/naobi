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
  Scenario: Adding two positive numbers with spacers
    Given script:
      """
      import standard;
      workflow main
      {
      println(5 + 10)
      }
      """
    Then got 15
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
    | 5.0     | 4.3     | 9.3    |
    | 100000  | 1000000 | 1100000|

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