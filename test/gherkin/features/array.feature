Feature: Array
  Scenario: Create array of integers
    Given script:
    """
    import standard;
    workflow main
    {
      array<integer> integers;
    }
    """