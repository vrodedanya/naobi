Feature: Exception
  Scenario: Catch cast exception
    Given script:
    """
    import standard;
    workflow main
    {
        integer temp = int("text");
        catch CastException
        {
            println("Catch exception");
            exit(0);
        }
        println(temp);
    }
    """
    Then got string "Catch exception"
    Then ends with the code 0
  Scenario: Create exception
    Given script:
    """
    import standard;
    exception TestCase;
    """
    Then ends with the code 0
  Scenario: Throw exception
    Given script:
    """
    import standard;
    exception TestCase;
    workflow main
    {
      throw TestCase("Desc");
      catch TestCase
      {
        println("Catch");
      }
    }
    """
    Then got string "Catch"
    Then ends with the code 0
  Scenario: Throw not existing exception
    Given script:
    """
    import standard;
    workflow main
    {
      throw TestCase("Desc");
      catch TestCase
      {
        println("Catch");
      }
    }
    """
    Then fails with compilation error 52
  Scenario: Print exception message
    Given script:
    """
    import standard;
    exception TestCase;
    workflow main
    {
      throw TestCase("Desc");
      catch TestCase
      {
        println(TestCase.description);
      }
    }
    """
    Then got string "Desc"
    Then ends with the code 0