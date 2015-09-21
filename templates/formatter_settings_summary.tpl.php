<h3><strong><?php print('Current Formatter Settings'); ?></strong></h3>

<ul>
<?php foreach($variables['form'] as $setting_key => $setting_value): ?>
  <li>
    <b><?php print($setting_value['#title']); ?>:</b>

    <?php if (gettype($setting_value['#default_value']) === 'string' || gettype($setting_value['#default_value']) === 'integer'): ?>
      <?php print($setting_value['#default_value']); ?>

    <?php elseif (gettype($setting_value['#default_value']) === 'array'): ?>
      <ul>
      <?php foreach($setting_value['#default_value'] as $inner_setting_key => $inner_setting_value): ?>
        <li>
          <?php if (gettype($inner_setting_value) === 'string' || gettype($inner_setting_value) === 'integer'): ?>
            <?php print($inner_setting_value); ?>
          <?php endif; ?>
        </li>
      <?php endforeach; ?>
      </ul>

    <?php endif; ?>
  </li>
<?php endforeach; ?>
</ul>