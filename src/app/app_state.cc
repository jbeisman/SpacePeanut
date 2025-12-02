
#include "app.hh"

AppState::AppState() {
	renderer = std::make_unique<Renderer>();
}

void AppState::initializeSimulation() {
	this->renderer->init(this->rshift, this->nsteps, this->nbods, this->ngrid, this->gmax);
	this->sim_initialized = true;
	this->current_redshift = this->renderer->current_redshift;
}

void AppState::updateSimulation() {
	if (this->pause_state == false) {
		this->renderer->update(this->evolution_speed, this->reverse_time);
		this->current_redshift = this->renderer->current_redshift;
	}
}

void AppState::displaySimulation(float aspect_ratio) {
	this->renderer->display(aspect_ratio, this->clip_factor, this->log_scale);
}

void AppState::resetSimulation() {
	this->pause_state = true;
	this->execute_sim_init = false;
	this->sim_initialized = false;
	this->change_color_palette = true;
	this->renderer->reset_simulator();
}

void AppState::changeColors() {
	this->renderer->change_color(this->color);
	this->change_color_palette = false;
}
void AppState::dragStart(const glm::vec2 &mouse_pos) {
	this->renderer->camera.start_drag(mouse_pos);
}

void AppState::dragUpdate(const glm::vec2 &mouse_pos, const glm::vec2 &view_size) {
	this->renderer->camera.update_drag(mouse_pos, view_size);
}

void AppState::dragEnd() {
	this->renderer->camera.end_drag();
}

void AppState::zoomUpdate(float scroll_offset) {
	this->renderer->camera.update_zoom(scroll_offset);
}