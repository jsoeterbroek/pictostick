/* Project specific Javascript goes here. */
const draggables = document.querySelectorAll(".activity");
const droppables = document.querySelectorAll(".swim-lane");
var activity_id= '0';
var emp_id = '0';


draggables.forEach((activity) => {
  activity.addEventListener("dragstart", () => {
    activity.classList.add("is-dragging");
  });
  activity.addEventListener("dragend", () => {
    activity.classList.remove("is-dragging");

    get_assign(activity_id, emp_id);

  });
});

droppables.forEach((zone) => {
  zone.addEventListener("dragover", (e) => {
    e.preventDefault();

    const bottomActivity = insertAboveActivity(zone, e.clientY);
    const curActivity = document.querySelector(".is-dragging");

    if (!bottomActivity) {
      zone.appendChild(curActivity);
    } else {
      zone.insertBefore(curActivity, bottomActivity);
    }
    activity_id = curActivity.id;
    emp_id = zone.id;
    });
});

const insertAboveActivity = (zone, mouseY) => {
  const els = zone.querySelectorAll(".activity:not(.is-dragging)");

  let closestActivity = null;
  let closestOffset = Number.NEGATIVE_INFINITY;


  els.forEach((activity) => {
    const { top } = activity.getBoundingClientRect();

    const offset = mouseY - top;

    if (offset < 0 && offset > closestOffset) {
      closestOffset = offset;
      closestActivity = activity;
    }
  });

  return closestActivity;
};

function get_assign(activity_id, emp_id){
    send_request(emp_id, activity_id);
}

function send_request(emp_id, activity_id){
    $.ajax({
      type: 'GET',
      url: `/myapp/activity-assign/${emp_id}/${activity_id}/`,
      failure: function(data){
        console.log('failure');
        console.log(data);
      },
    });
  }
//Inspired by https://github.com/TomIsLoading/drag-and-drop-kanban
