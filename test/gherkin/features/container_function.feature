Feature: Functions for working with containers (sequence of smthg)
  Scenario: Get length of string
    Given script:
    """
    import standard;
    workflow main
    {
      println(len("str"));
    }
    """
    Then got integer 3
    Then ends with the code 0
  Scenario: Get second symbol
    Given script:
    """
    import standard;
    workflow main
    {
      println(get("text", 1));
    }
    """
    Then got string "e"
    Then ends with the code 0
  Scenario: Change third symbol
    Given script:
    """
    import standard;
    workflow main
    {
      println(set("text", "|", 2));
    }
    """
    Then got string "te|t"
    Then ends with the code 0
  Scenario: Append element
    Given script:
    """
    import standard;
    workflow main
    {
      string text = "Hello ";
      text = append(text, "world!");
      println(text);
    }
    """
    Then got string "Hello world!"
    Then ends with the code 0