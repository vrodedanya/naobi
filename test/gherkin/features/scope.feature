Feature: Code scope
  Scenario: Try to create existing in parent scope variable in local if scope
    Given script:
    """
    import standard;
    workflow main
    {
      integer temp = 0;
      if true
      {
        integer temp = 10;
      }
    }
    """
    Then fails with compilation error 51
    Scenario: Using extern variable in if local scope
      Given script:
      """
      import standard;
      workflow main
      {
        integer temp = 10;
        if true
        {
          println(temp);
        }
        println(temp);
      }
      """
      Then got integer 10
      Then got integer 10
      Then ends with the code 0
    Scenario: Create variable in 'if' local scope and got error when try to use it out of scope
      Given script:
      """
      import standard;
      workflow main
      {
        if true
        {
          integer in = 10;
        }
        println(in);
      }
      """
      Then fails with compilation error 52

  Scenario: Try to create existing in parent scope variable in local 'for' scope
    Given script:
    """
    import standard;
    workflow main
    {
      integer temp = 0;
      for integer i in (0, 1)
      {
        integer temp = 10;
      }
    }
    """
    Then fails with compilation error 51
  Scenario: Using extern variable in 'for' local scope
    Given script:
      """
      import standard;
      workflow main
      {
        integer temp = 10;
        for integer i in (0, 1)
        {
          println(temp);
        }
        println(temp);
      }
      """
    Then got integer 10
    Then got integer 10
    Then ends with the code 0
  Scenario: Create variable in 'for' local scope and got error when try to use it out of scope
    Given script:
      """
      import standard;
      workflow main
      {
        for integer i in (0, 1)
        {
          integer in = 10;
        }
        println(in);
      }
      """
    Then fails with compilation error 52