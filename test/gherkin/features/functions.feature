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
    Then fails with compilation error and code 52

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

  Scenario: Function that takes arguments and returns it
    Given script:
    """
    import standard;
    function pass(integer value) -> integer
    {
      return value;
    }
    workflow main
    {
      println(pass(5));
    }
    """
    Then got integer 5
    Then ends with the code 0

  Scenario: Function that add two integers and return the sum
    Given script:
    """
    import standard;
    function sum(integer first, integer second) -> integer
    {
      return first + second;
    }
    workflow main
    {
      println(sum(first: 10, second: 40));
    }
    """
    Then got integer 50
    Then ends with the code 0

  Scenario: Provided 1 argument but need 2
    Given script:
    """
    import standard;
    function sum(integer first, integer second) -> integer
    {
      return first + second;
    }
    workflow main
    {
      println(sum(10));
    }
    """
    Then fails with compilation error and code 57

  Scenario: Multiple return in function
    Given script:
    """
    import standard;
    function number(integer var) -> integer
    {
      if (var == 1)
      {
        return 1;
      }
      else
      {
        return 2;
      }
      return 3;
    }
    workflow main
    {
      println(number(2));
    }
    """
    Then got integer 2
    Then ends with the code 0

  Scenario: Wrong argument type
    Given script:
    """
    import standard;
    function printValue(integer val)
    {
      println("Integer");
    }
    workflow main
    {
      printValue(5.0);
    }
    """
    Then fails with compilation error and code 53

  Scenario: Function overload simple
    Given script:
    """
    import standard;
    function printValue(integer val)
    {
      println("Integer");
    }
    function printValue(float val)
    {
      println("Float");
    }
    workflow main
    {
      printValue(5);
      printValue(5.0);
    }
    """
    Then got string "Integer"
    Then got string "Float"
    Then ends with the code 0

  Scenario: Function overload named arguments
    Given script:
    """
    import standard;
    function printValue(integer val, float second)
    {
      println("Integer");
    }
    function printValue(integer val, integer second)
    {
      println("Float");
    }
    workflow main
    {
      printValue(second: 5, val: 6);
      printValue(second: 5.0, val: 1);
    }
    """
    Then got string "Float"
    Then got string "Integer"
    Then ends with the code 0

  Scenario: Function overload wrong types
    Given script:
    """
    import standard;
    function printValue(integer val, float second)
    {
      println("Integer");
    }
    function printValue(integer val, integer second)
    {
      println("Float");
    }
    workflow main
    {
      printValue(second: 5, val: "String");
      printValue(second: 5.0, val: 1);
    }
    """
    Then fails with compilation error and code 53

  Scenario: Function already exists
    Given script:
    """
    import standard;
    function printValue(integer val, float second)
    {
      println("Integer");
    }
    function printValue(integer val, float second)
    {
      println("Float");
    }
    workflow main
    {
      printValue(second: 5, val: "String");
      printValue(second: 5.0, val: 1);
    }
    """
    Then fails with compilation error and code 51

  Scenario: Function overloading with return type
    Given script:
    """
    import standard;
    function return_value(integer val) -> integer
    {
      println("Integer");
      return val;
    }
    function return_value(float val) -> float
    {
      println("Float");
      return val;
    }
    workflow main
    {
      return_value(10);
      return_value(5.0);
    }
    """
    Then got string "Integer"
    Then got string "Float"
    Then ends with the code 0
  Scenario: String argument with ':'. Issue #10
    Given script:
    """
    import standard;
    function pri(string val)
    {
      println(val);
    }
    workflow main
    {
      pri("str: test");
    }
    """
    Then got string "str: test"
    Then ends with the code 0
