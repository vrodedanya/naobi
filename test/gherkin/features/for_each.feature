Feature: For each loop

  Scenario: Print range from 0 to 9
    Given script:
    """
    import standard;
    workflow main
    {
      for integer i in (0, 5)
      {
        println(i);
      }
    }
    """
    Then got integer 0
    Then got integer 1
    Then got integer 2
    Then got integer 3
    Then got integer 4
    Then ends with the code 0

  Scenario: Print power of 2
    Given script:
    """
    import standard;
    function pow(float number, integer power) -> float
    {
      float temp = 1.0;
      for integer i in (0, power)
      {
        temp = temp * number;
      }
      return temp;
    }
    workflow main
    {
      println(pow(2.0, 10));
      println(pow(2.0, 5));
      println(pow(3.0, 2));
    }
    """
    Then got float 1024.0
    Then got float 32.0
    Then got float 9.0
    Then ends with the code 0
  Scenario: Wrong type in generator
    Given script:
    """
    workflow main
    {
      for integer i in (1.0, 15.0)
      {
      }
    }
    """
    Then fails with compilation error 53
  Scenario: Float value
    Given script:
    """
    import standard;
    workflow main
    {
      for float i in (1.0, 5.0)
      {
        println(i);
      }
    }
    """
    Then got float 1.0
    Then got float 2.0
    Then got float 3.0
    Then got float 4.0
    Then ends with the code 0