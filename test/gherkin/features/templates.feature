Feature: Template functions
  Scenario: Template sum function
    Given script:
    """
    import standard;
    function<T> sum(T first, T second) -> T
    {
      return first + second;
    }
    workflow main
    {
      println(sum(5, 5));
      println(sum(5.4, 4.6));
    }
    """
    Then got integer 10
    Then got float 10.0
    Then ends with the code 0
  Scenario: Function with two template arguments
    Given script:
    """
    import standard;
    function<T1, T2> pr(T1 first, T2 second)
    {
      println(first);
      println(second);
    }
    workflow main
    {
      pr("Value", 5.5)
    }
    """
    Then got string "Value"
    Then got float 5.5
    Then ends with the code 0