
std::string printMail ( const std::list<CMail> & all )
{
  std::ostringstream oss;
  for ( const auto & mail : all )
    oss << mail << "\n";
  return oss . str ();
}