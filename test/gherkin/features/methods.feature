Feature: Types' methods
  Functions which can be called like that:
  var.method(vars...);
  'var' is the first argument of 'method' function

  Scenario: Method with 1 argument
    Given script:
    """
    import standard;
    function print(integer argument)
    {
      println(argument);
    }

    workflow main
    {
      integer var = 5;
      var.print();
    }
    """
    Then got integer 5
    Then ends with the code 0

  Scenario: Method 'add' with two arguments that returns sum of the numbers
    Given script:
    """
    import standard;
    function add(integer first, integer second) -> integer
    {
      return first + second;
    }

    workflow main
    {
      integer var = 5;
      println(var.add(10));
    }
    """
    Then got integer 15
    Then ends with the code 0

  Scenario: Error if we try to pass argument to 'object'
    Given script:
    """
    import standard;
    function add(integer first, integer second) -> integer
    {
      return first + second;
    }

    workflow main
    {
      integer var = 5;
      println(var.add(first: 10));
    }
    """
    Then fails with compilation error 57

  Scenario: Error if we doesn't pass value
    Given script:
    """
    import standard;
    function add(integer first, integer second) -> integer
    {
      return first + second;
    }

    workflow main
    {
      integer var = 5;
      println(var.add());
    }
    """
    Then fails with compilation error 57

  Scenario: Call like method and function
    Given script:
    """
    import standard;
    function add(integer first, integer second) -> integer
    {
      return first + second;
    }

    workflow main
    {
      integer var = 5;
      println(var.add(10));
      println(add(var, 10));
    }
    """
    Then got integer 15
    Then got integer 15
    Then ends with the code 0

  Scenario: Function result as object of method
    Given script:
    """
    import standard;
    function add(integer first, integer second) -> integer
    {
      return first + second;
    }

    function print(integer argument)
    {
      println(argument);
    }

    workflow main
    {
      integer var = 5;
      var.add(10).print();

      print(add(var, 10));
    }
    """
    Then got integer 15
    Then ends with the code 0