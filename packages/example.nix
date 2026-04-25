{
  writeShellApplication,
  cowsay,
  fortune,
  format ? "dragon",
}:
writeShellApplication {
  name = "example";
  runtimeInputs = [cowsay fortune];
  text = ''
    fortune | cowsay -f ${format}
  '';
}
