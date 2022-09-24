Feature: Reference type
  Scenario: Reference to int
    Given script:
    """
    workflow main
    {
      integer var = 5;
      integer& ref = var;
    }
    """
    Then ends with the code 0
  Scenario: Reference to float
    Given script:
    """
    workflow main
    {
      float var = 5;
      float& ref = var;
    }
    """
    Then ends with the code 0
  Scenario: Argument reference
    Given script:
    """
    function zeroize(integer& number)
    {
      number = 0;
    }
    workflow main
    {
      integer t = 0;
      zeroize(t);
    }
    """
    Then ends with the code 0