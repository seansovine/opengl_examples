#ifndef SHADERS_H
#define SHADERS_H

const char *vertexShaderSource = R"TST(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  void main()
  {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)TST";

const char *fragmentShaderSourceOne = R"TST(
  #version 330 core
  out vec4 FragColor;
  void main()
  {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  }
)TST";

const char *fragmentShaderSourceTwo = R"TST(
  #version 330 core
  out vec4 FragColor;
  void main()
  {
    FragColor = vec4(0.0f, 0.8f, 0.2f, 1.0f);
  }
)TST";

#endif
