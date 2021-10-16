#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition, vColor, vNormal, texCoord, uModelViewMatrix, normalMatrix, viewMatrix;
extern GLuint light1, light2, spotlight1; // for turning lights
extern int light1_value, light2_value, spotlight1_value;
extern std::vector<glm::mat4> matrixStack;
extern glm::mat3 normal_matrix;
extern glm::mat4 view_matrix; // for the normals part
extern float increment_trans;
extern std::ifstream keyframes_in;
extern std::vector<float> current_frame;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, glm::vec4* a_normals, glm::vec2* a_tex_coords, std::size_t v_size, std::size_t c_size, std::size_t n_size, std::size_t t_size, GLuint texture){

		num_vertices = num_v;

		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		normal_buffer_size = n_size;
		texture_buffer_size = t_size;

		tex = texture;
		// initialize vao and vbo of the object;

		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + normal_buffer_size + texture_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, normal_buffer_size, a_normals );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + normal_buffer_size, texture_buffer_size, a_tex_coords );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size));

		glEnableVertexAttribArray( texCoord );
		glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size + normal_buffer_size));


		// set parent
		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices(){
		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));
	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);
	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::render(){

		glUniform1i(light1, light1_value);
		glUniform1i(light2, light2_value);
  		glUniform1i(spotlight1, spotlight1_value);
  		glBindTexture(GL_TEXTURE_2D, tex);

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		normal_matrix = glm::transpose (glm::inverse(glm::mat3(*ms_mult)));
		glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glBindVertexArray (vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory 
		delete ms_mult;
	}

	void HNode::render_tree(){
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();
	}

	void HNode::inc_rx(){
		rx++;
		update_matrices();
	}

	void HNode::inc_ry(){
		ry++;
		update_matrices();
	}

	void HNode::inc_rz(){
		rz++;
		update_matrices();
	}

	void HNode::dec_rx(){
		rx--;
		update_matrices();
	}

	void HNode::dec_ry(){
		ry--;
		update_matrices();
	}

	void HNode::dec_rz(){
		rz--;
		update_matrices();
	}

	void HNode::inc_tx(){
		tx+=increment_trans;
		update_matrices();
	}

	void HNode::inc_ty(){
		ty+=increment_trans;
		update_matrices();
	}

	void HNode::inc_tz(){
		tz+=increment_trans;
		update_matrices();
	}

	void HNode::dec_tx(){
		tx-=increment_trans;
		update_matrices();
	}

	void HNode::dec_ty(){
		ty-=increment_trans;
		update_matrices();
	}

	void HNode::dec_tz(){
		tz-=increment_trans;
		update_matrices();
	}

	GLfloat HNode::get_rx(){
		return rx;
	}

	GLfloat HNode::get_ry(){
		return ry;
	}

	GLfloat HNode::get_rz(){
		return rz;
	}

	GLfloat HNode::get_tx(){
		return tx;
	}

	GLfloat HNode::get_ty(){
		return ty;
	}

	GLfloat HNode::get_tz(){
		return tz;
	}

	void HNode::set_rx(float value){
		rx = value;
	}

	void HNode::set_ry(float value){
		ry = value;
	}

	void HNode::set_rz(float value){
		rz = value;
	}

	void HNode::set_tx(float value){
		tx = value;
	}

	void HNode::set_ty(float value){
		ty = value;
	}

	void HNode::set_tz(float value){
		tz = value;
	}

	void HNode::save_current_frame(std::ofstream & keyframes){
		keyframes<<get_tx()<<" "<<get_ty()<<" "<<get_tz()<<" "; // information about translation
		keyframes<<get_rx()<<" "<<get_ry()<<" "<<get_rz()<<" "; // information about rotation

		for(int i=0;i<children.size();i++){
			children[i]->save_current_frame(keyframes);
		}
	}

	void HNode::load_frame_vector_hierarchy(std::vector<float> & frame_vector){
		float input;
		for(int i=0; i<6; i++){ // since translation 3 and rotation 3
			keyframes_in>>input;
			frame_vector.push_back(input);
		}

		for(int i=0;i<children.size();i++){
			children[i]->load_frame_vector_hierarchy(frame_vector);
		}
	}

	int HNode::load_values_from_vectorframe_hierarchy(int index){
		set_tx(current_frame[index]);
		index++;
		set_ty(current_frame[index]);
		index++;
		set_tz(current_frame[index]);
		index++;
		set_rx(current_frame[index]);
		index++;
		set_ry(current_frame[index]);
		index++;
		set_rz(current_frame[index]);
		index++;

		for(int i=0;i<children.size();i++){
			index = children[i]->load_values_from_vectorframe_hierarchy(index);
		}
		update_matrices();

		return index;
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}