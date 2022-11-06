import Home from "./pages/Home";
import Saved from "./pages/Saved";
import File from "./pages/File"

const routes = [
	{
		path: "/",
		name: "home",
		component: Home,
	},
	{
		path: "/saved",
		name: "saved",
		component: Saved,
	},
	{
		path: "/file",
		name: "file",
		component: File,
	},
];

export default routes;
