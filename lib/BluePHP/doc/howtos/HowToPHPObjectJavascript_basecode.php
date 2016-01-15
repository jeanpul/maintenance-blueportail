<!DOCTYPE html>
<html>
<head>
<script src="/BluePHP/js/BluePHP.js" type="text/javascript"></script>
</head>
<body>

<div id="resCall">
</div>

<script type="text/javascript">
var gui =  BluePHP.newObject("BlueProjectPHP.ComFacturesInterface");
$('#resCall').html(gui.render()); 
</script>
</body>
</html>
