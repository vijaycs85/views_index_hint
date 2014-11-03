<?php

/**
 * @file
 *
 * Contains ViewsIndexHint.
 */

/**
 * The extender class for Select queries to add index hint.
 */
class ViewsIndexHint extends SelectQueryExtender {

  /**
   * @var
   */
  protected $index_hint;

  /**
   * @var
   */
  protected $base_table;

  /**
   * Setter for base table.
   *
   * @param $base_table
   *
   * @return $this
   */
  public function setBaseTable($base_table) {
    $this->base_table = $base_table;
    return $this;
  }

  /**
   * Setter for index hint.
   *
   * @param $index_hint
   *
   * @return $this
   */
  public function setIndexHint($index_hint) {
    $this->index_hint = $index_hint;
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function execute() {
    // If validation fails, simply return NULL.
    // Note that validation routines in preExecute() may throw exceptions instead.
    if (!$this->preExecute()) {
      return NULL;
    }

    $args = $this->getArguments();
    $query = (string) $this->query;
    // There is no option to add the index hint except overriding
    // Execute as Drupal 7 has SelectQuery object on all places
    // (unlike Drupal 6 static query).
    // @see https://github.com/nrambeck/Drupal-Views-Index-Hint for Drupal 6
    // equivalent implementation.
    $search = '{'. $this->base_table .'} '. $this->base_table;
    $replace = $search .' USE INDEX(' . $this->index_hint . ')';
    $query = str_replace($search, $replace, $query);
    // @TODO: Skipped queryOptions as it is not in scope. See if there way to
    // add it back.
    // @see SelectQuery::execute.
    return $this->connection->query($query, $args);
  }

}
