Feature: Users functions
  User can create functions and pass arguments with different types. Function can return the value
  Scenario: Function that only prints message and doesn't get or return the value
    Given script:
    """
    import standard;
    function printHello()
    {
      println("Hello");
    }
    workflow main
    {
      printHello();
    }
    """
    Then got string "Hello"
    Then ends with the code 0