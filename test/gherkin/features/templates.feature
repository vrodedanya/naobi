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