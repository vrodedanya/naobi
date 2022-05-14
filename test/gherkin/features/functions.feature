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
  Scenario: Calling not existed function
    Given script:
    """
    workflow main
    {
      printMessage();
    }
    """
    Then ends with the code 1
  Scenario: Function takes integer and prints it
    Given script:
    """
    import standard;
    function printInteger(integer var)
    {
      println(var);
    }
    workflow main
    {
      printInteger(var: 10);
    }
    """
    Then got integer 10
    Then ends with the code 0
  Scenario: Function takes two integer arguments and print sum
    Given script:
    """
    import standard;
    function printSum(integer first, integer second)
    {
      println(first + second);
    }
    workflow main
    {
      printSum(first: 10, second: 40);
    }
    """
    Then got integer 50
    Then ends with the code 0
  Scenario: Function that returns value
    Given script:
    """
    import standard;
    function five() -> integer
    {
      return 5;
    }
    workflow main
    {
      println(five());
    }
    """
    Then got integer 5
    Then ends with the code 0