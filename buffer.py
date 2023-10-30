import resource

def get_available_buffer_size():
  """Returns the maximum size of a buffer that can be allocated."""
  rlim = resource.getrlimit(resource.RLIMIT_STACK)
  return rlim[1]

if __name__ == "__main__":
  print("The maximum buffer size is {} bytes".format(get_available_buffer_size()))
