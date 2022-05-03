Feature: Arithmetic expression
  Scenario: Sum
    Given script:
      """
      import standard;

      workflow main
      target begin
      {
        println(5+5);
      }
      """
    Then got 10